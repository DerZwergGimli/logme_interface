<template>
  <div class="mx-3 flex flex-col space-y-3">
    <div
      id="glowElement"
      v-for="(sensor, idx) in sensorStore.sensors_power"
      :key="idx"
      class=""
    >
      <div
        class="flex flex-row border-2 rounded-lg p-2 bg-gradient-to-r from-cyan-400 to-blue-400 dark:from-cyan-800 dark:to-blue-800"
      >
        <div class="flex flex-row basis-1/2 items-center justify-center">
          <smart-meter-icon class="w-24 dark:text-white" />

          <div class="flex flex-col w-full">
            <p class="flex w-full text-2xl">{{ sensor.name }}</p>
            <p class="flex w-full text-md">{{ sensor.description }}</p>
          </div>
        </div>
        <div class="flex flex-col basis-1/2 justify-center space-y-2">
          <value-element
            :value="sensor.count.toString()"
            icon="numbers"
            description="SensorCount"
            unit="kWh"
            :editable="false"
          />
          <value-element
            :value="sensor.power.toString()"
            icon="power-plug"
            description="SenorPower"
            unit="W"
            :editable="false"
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
