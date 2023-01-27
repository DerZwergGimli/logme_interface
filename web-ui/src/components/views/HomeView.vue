<template>
  <div class="mx-3">
    <div
      v-for="(sensor, idx) in sensorStore.sensors"
      :key="idx"
      class="space-y-5"
    >
      <div
        class="flex flex-row border-2 rounded-lg p-2 bg-gradient-to-r from-cyan-400 to-blue-400 dark:from-cyan-800 dark:to-blue-800"
      >
        <div class="flex flex-row basis-1/2 items-center justify-center">
          <div class=""><smart-meter-icon class="w-24 dark:text-white" /></div>
          <p class="flex w-full text-2xl">{{ sensor.name }}</p>
        </div>
        <div class="flex flex-col basis-1/2 justify-center space-y-2">
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
import SmartMeterIcon from '../icons/SmartMeterIcon.vue';

const sensorStore = useSensorStore();
</script>
