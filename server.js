class Data {
  constructor(id, data) {
    this.id = id;
    this.data = data;
  }
}

// @brief Server that runs on Azure VM and act like an observer how the data
// transmitted from sensor to broker. This server also hold an instance to 
// Firestore to log updated data and also has priviledge (if granted) to turn 
// on/off devices autonomously.
// @brief To run this server, implementter must has enviroment variable that
// hold an credential file to access firestore. On linux machine, set as:
// ##     export GOOGLE_APPLICATION_CREDENTIALS="[PATH]"
// Also, install @google-clound/storage using npm
class Server {

  constructor(db) {
    this.queue = new Array();
    this.db = db;
    this.automode = false;
    this.areas = {};
    this.client = this.config_mqtt_client(this.db);
  }

  // @brief Work should be done after every [interval] time. Workflow is:
  //    - Log sensor data (in this.queue) to Firestore (1)
  //    - Check autoode at Firestore (2)
  //    - If automode is true: (3)
  //      - For each area current humid, get upperbound humid, lowerbound
  //      humid, and current humid (is stored at this.areas), devices at that 
  //      area. Turn on/off device usign Mqtt and log the action at '/DevicesLog' 
  //    - Clear the queue
  async update() {
    /* (1) */
    this.db.log(this.queue);
    /* (2) */
    this.automode = await this.db.get_automode_flag();
    if (this.automode) {
      /* (3) */
      for (var area_id in this.areas) {
        var did = await this.db.get_device(area_id);
        //console.log(did);
       
        /* bounds is an array of:
         * [upper_temp, lower_temp, upper_humi, lower_humi] 
         *
         * Only last 2 is used
         */
        var bounds = await this.db.get_area_bounds(area_id);
        var area_humi = this.areas[area_id];
        var low_humi = bounds[3];
        var high_humi = bounds[2];
        if (area_humi < low_humi) {
          this.client.publish('Topic/Speaker', 
            `[\n{ "device_id": "${did}",\n"values":["1", "5000"]\n}\n]`);
          this.db.log_device(new Data (did, [1, 5000]));
        }
        else if (area_humi > high_humi) {
          this.client.publish('Topic/Speaker', 
            `[\n{ "device_id": "${did}",\n"values":["0", "0"]\n}\n]`);
          this.db.log_device(new Data (did, [0, 0]));
        }
      }
    }
    this.queue = [];
  }

  // @brief Set up MQTT Client using the config file, also set up behaviour
  // when get an messages. In particular:
  //  - If topic is 'Topic/TempHumi', push it to the queue.
  //  - else ignore
  config_mqtt_client(db) {
    var mqtt = require('mqtt');
    var config = require('./config.json');
    var client = mqtt.connect(
      {
        host: config.host,
        port: config.port,
        clientId: config.device_id,
        username: config.username,
        password: config.password
      }
    );

    var up_topic = config.up_topic;
    var down_topic = config.down_topic;

    client.on('connect', async function() {
      /* down_topic is set inside config file */
      client.subscribe(down_topic);
    });

    var queue = this.queue;
    var areas = this.areas;
    var db = this.db;

    client.on('message', async function(topic, payload) {
      if (topic == 'Topic/TempHumi') {
        var data_list = JSON.parse(payload.toString());
        data_list.forEach(async function (data) {
          /* data[0]: temp, data[1]: humid */
          var id = data.device_id;
          var data = data.values;
          queue.push(new Data (id, data));
          /* Update area current humid */
          var area_id = await db.get_area(id);
          areas[area_id] = data[1];
        });
      }
    });

    return client;
  }
}

const {Firestore} = require('@google-cloud/firestore');
const firestore = new Firestore();

// @brief This Class hold an connection to Firestore and reponsible for
// logging, reading from Firestore, there are some helper functions to get
// area_id, device_id, ....
class DB {
  constructor(firestore) {
    this.firestore = firestore;
  }

  // @brief Log Device data only. Data format is: 
  // {device_id, data[0]: state, data[1]: Range/Values}
  async log_device(data) {
    this.firestore.collection('DevicesLog').add(
      {
        DID: data.id,
        Time: Firestore.FieldValue.serverTimestamp(),
        Range: data.data[1]
      });
  }
  
  // @brief Log sensor data only. A list has some data, Data format is:
  // {device_id, data[0]: temp, data[1]: humid}
  async log(data_list) {
    data_list.forEach(data => {
      /* Another check device id */
      if (data.id.charAt(0) == 'S') {
        this.firestore.collection('SensorLog').add(
          {
            SID: data.id,
            Humid: data.data[1],
            Temp: data.data[0],
            Time: Firestore.FieldValue.serverTimestamp()
          });
      }
    });
  }

  // @brief Check automode from firebase
  // @params 
  // @return an flag that idicate whether automode is activated or not
  async get_automode_flag() {
    var snapshot = await this.firestore.collection('Automode').doc('Auto').get();
    if (snapshot.exists) {
      return snapshot.get('status');
    }
  }
  
  // @brief set automode from firebase
  // @params 
  // @return 
  //
  // @deprecated
  set_automode_flag(flag) {
    this.firestore.collection('Automode').doc('Auto')
      .set({status: flag})
      .then(res => console.log('automode changed'));
  }

  // @brief Get area upperbound and lower bound
  // @param Area Id
  // @return array of [upper_temp, lower_temp, upper_humid, lower_humid]
  async get_area_bounds(area_id) {
    var snapshot = await this.firestore.collection('AreaPlant').doc(area_id).get();
    if (snapshot.exists) {
      var bounds = [0, 0];
      bounds.push(snapshot.get('upperbound'));
      bounds.push(snapshot.get('lowerbound'));
      return bounds;
    }
  }


  // @brief Find area id base on device id
  // @param: device id or sensor id or nothing
  // @return: an area that has id OR list off all areas if nothing is provided
  async get_area(id) {
    if (id != null) {
      if (id.charAt(0) == 'D') {
        var snapshot = await this.firestore.collection('Devices').doc(id).get();
        if (snapshot.exists) {
          return snapshot.get('AreaId');
        }
      }
      else {
        var snapshot = await this.firestore.collection('HumidSensor').doc(id).get();
        if (snapshot.exists) {
          return snapshot.get('AreaId');
        }
      }
    }
    else {
      var ref = this.firestore.collection('AreaPlant').listDocuments();
      var id_list = [];
      ref.forEach(area => id_list.push(area.id));
      return id_list;
    }
  }

  // @brief Get device id base on area id. Not work with sensor
  // @param areaid or nothing
  // @return an device in that area or all devices if nothing is provieded
  async get_device(area_id) {
    console.log(area_id);
    if (area_id != null) {
      var devices = this.firestore.collection('Devices');
      var query_snapshot = await devices.where('AreaId', '==', area_id).get();
      if (query_snapshot.docs[0].exists){
        return query_snapshot.docs[0].id;
      }
    }
    else {
      var snapshot = await this.firestore.collection('Devices').get();
      var id_list = [];
      snapshot.forEach(doc => id_list.push(doc.id));
      return id_list;
    }
  }
}

var db = new DB(firestore);
var server = new Server(db);

function update() {
  server.update();
}
setInterval(update, 60000*5);
