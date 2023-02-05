import { defineStore } from 'pinia';
import { I_Sensor } from './interfaces/I_Sensor';

export const useSensorStore = defineStore('sensors', {
  state: () => ({ sensors: [] as Array<I_Sensor> }),

  actions: {
    async fetch() {
      await fetch(APP_API_URL + '/sensors')
        .then(resp => resp.json())
        .then(json => {
          console.log(json);
          this.sensors = json;
          // this.sensors = json as Array<Sensor>;
        });
      console.info('sensorStore fetch');
    },
  },
});
