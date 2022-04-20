import React, { useState, useEffect } from 'react';
import './App.css';

function App() {
  const [supportsBluetooth, setSupportsBluetooth] = useState(false);
  const [isDisconnected, setIsDisconnected] = useState(true);
  const [batteryLevel, setBatteryLevel] = useState(null);
  const Arduino1Service = "2384bddc-886f-11ec-a8a3-0242ac120000";
  const Arduino1Control = "2384bddc-886f-11ec-a8a3-0242ac120001";
  //Don't have IDs for Ard. 2 yet

  // When the component mounts, check that the browser supports Bluetooth
  useEffect(() => {
    if (navigator.bluetooth) {
      setSupportsBluetooth(true);
    }
  }, []);

  /**
   * Let the user know when their device has been disconnected.
   */
  const onDisconnected = (event) => {
    alert(`The device ${event.target} is disconnected`);
    setIsDisconnected(true);
  }

  /**
   * Update the value shown on the web page when a notification is
   * received.
   */
  const handleCharacteristicValueChanged = (event) => {
    setBatteryLevel(event.target.value.getUint8(0) + '%');
  }

  const connectToDeviceAndSubscribeToUpdatesArduino1 = async () => {
    try {
      // Search for Bluetooth devices that advertise a battery service
      const device = await navigator.bluetooth
        .requestDevice({
          filters: [{services: [Arduino1Service]}]
        });

      setIsDisconnected(false);

      // Add an event listener to detect when a device disconnects
      device.addEventListener('gattserverdisconnected', onDisconnected);

      // Try to connect to the remote GATT Server running on the Bluetooth device
      const server = await device.gatt.connect();

      // Get the service from the Bluetooth device
      const service = await server.getPrimaryService(Arduino1Service);

      // Get the characteristic from the Bluetooth device
      const characteristic = await service.getCharacteristic(Arduino1Control);

      // Subscribe to  notifications
      characteristic.startNotifications();

      // When the changes, call a function
      characteristic.addEventListener('characteristicvaluechanged',
                                  handleCharacteristicValueChanged);
      
      // Read the value
      const reading = await characteristic.readValue();

      // Show the initial reading on the web page
      setBatteryLevel(reading + ' is the Value');
    } catch(error) {
      console.log(`There was an error: ${error}`);
    }
  }
  const MoveUpArduino1 = async () => {
    try {
      const device = await navigator.bluetooth
        .requestDevice({
          filters: [{services: [Arduino1Service]}]
        });
      const server = await device.gatt.connect();
      const service = await server.getPrimaryService(Arduino1Service);
      const characteristic = await service.getCharacteristic(Arduino1Control);
      device.writeCharacteristicWithResponseForService(Arduino1Service, Arduino1Control, Buffer.from("22").toString("base64"));


    } catch(error) {
      console.log(`There was an error: ${error}`);
    }
  };

  const connectToDeviceAndSubscribeToUpdatesArduino2 = async () => {
    try {
      // Search for Bluetooth devices that advertise a specific service
      const device = await navigator.bluetooth
        .requestDevice({
          filters: [{services: [Arduino1Service]}]
        });

      setIsDisconnected(false);

      // Add an event listener to detect when a device disconnects
      device.addEventListener('gattserverdisconnected', onDisconnected);

      // Try to connect to the remote GATT Server running on the Bluetooth device
      const server = await device.gatt.connect();

      // Get the ervice from the Bluetooth device
      const service = await server.getPrimaryService(Arduino1Service);

      // Get the  characteristic from the Bluetooth device
      const characteristic = await service.getCharacteristic(Arduino1Control);

      // Subscribe to notifications
      characteristic.startNotifications();

      // When the  changes, call a function
      characteristic.addEventListener('characteristicvaluechanged',
                                  handleCharacteristicValueChanged);
      
      // Read the value
      const reading = await characteristic.readValue();

      // Show the initial reading on the web page
      setBatteryLevel(reading + ' is the Value');
    } catch(error) {
      console.log(`There was an error: ${error}`);
    }
  }
  const MoveUpArduino2 = async () => {
    try {
      const device = await navigator.bluetooth
        .requestDevice({
          filters: [{services: [Arduino1Service]}]
        });
      const server = await device.gatt.connect();
      const service = await server.getPrimaryService(Arduino1Service);
      const characteristic = await service.getCharacteristic(Arduino1Control);
      device.writeCharacteristicWithResponseForService(Arduino1Service, Arduino1Control, Buffer.from("22").toString("base64"));


    } catch(error) {
      console.log(`There was an error: ${error}`);
    }
  };

  return (
    <div className="App">
      <h1>Agile Robotics Lab - Tensegrity Robot</h1>
      {supportsBluetooth && !isDisconnected &&
        <div>
        <button onClick={MoveUpArduino1}>Move Arduino 1</button>
            <p>Characteristic Value: {batteryLevel}</p>
        </div>
      }
      {supportsBluetooth && !isDisconnected &&
        <div>
        <button onClick={MoveUpArduino2}>Move Arduino 2</button>
            <p>Characteristic Value: {batteryLevel}</p>
        </div>
      }
      {supportsBluetooth && isDisconnected &&
        <button onClick={connectToDeviceAndSubscribeToUpdatesArduino1}>Connect to Arduino 1</button>
      }
      {supportsBluetooth && isDisconnected &&
        <button onClick={connectToDeviceAndSubscribeToUpdatesArduino2}>Connect to Arduino 2</button>
      }
      {!supportsBluetooth &&
        <p>This browser doesn't support the Web Bluetooth API</p>
      }
    </div>
  );
}

export default App;
