import { defineStore } from 'pinia';
import { I_Sensor } from './interfaces/I_Sensor';

export const useSensorStore = defineStore('sensors', {
  state: () => ({ sensors: [] as Array<I_Sensor> }),

  actions: {},
});
