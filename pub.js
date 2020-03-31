//MQTT publisher
var mqtt = require('mqtt')
var client = mqtt.connect('mqtt://localhost:1883')
var topic = 'Hola'
var message = 'Hello'

client.on('connect', ()=>{
    setInterval(()=>{
        client.publish(topic, message)
        console.log('message sent', message)
    },5000)
})