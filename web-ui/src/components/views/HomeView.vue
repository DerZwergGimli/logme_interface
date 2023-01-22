<template>
  <div class="m-3">
    <div
      v-for="(sensor, idx) in sensorStore.sensors"
      :key="idx"
      class="space-y-5"
    >
      <div
        class="flex flex-row border-2 rounded-lg p-2 bg-gradient-to-r from-cyan-500 to-blue-500"
      >
        <div class="flex basis-1/2 items-center justify-center">
          {{ sensor.name }}
        </div>
        <div class="basis-1/2 flex-col space-y-1">
          <value-element
            :value="sensor.count.toString()"
            icon="numbers"
            description="SensorCount"
            unit="kWh"
          />
          <value-element
            :value="sensor.power.toString()"
            icon="power-plug"
            description="SenorPower"
            unit="W"
          />
        </div>
      </div>
      <div></div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { useSensorStore } from '../../stores/SensorStore.js';
import ValueElement from '../Elements/Sensors/ValueElement.vue';


const sensorStore = useSensorStore();
</script>
