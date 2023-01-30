import { defineStore } from 'pinia';
import { Sensor } from './type_Sensor';

export const useSensorStore = defineStore('sensors', {
  state: () => ({ sensors: [] as Array<Sensor> }),

  actions: {
    async fetch() {
      await fetch(APP_API_URL + '/sensors')
        .then(resp => resp.json())
        .then(json => {
          console.log(json)
          this.sensors = json
         // this.sensors = json as Array<Sensor>;
        });
      console.info("sensorStore fetch")
      this.create_dummy_data()
    },
    create_dummy_data() {
      this.sensors.forEach(sensor => {
        for (let i = 0; i < 100; i++) {
          sensor.chart_data?.push(i);
          sensor.chart_timestamps?.push(Date.now());
        }
      })
    },
  },
});
