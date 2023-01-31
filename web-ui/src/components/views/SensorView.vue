<template>
  <div class="flex flex-col" id="glowElement">
    <Accordion>
      <accordion-panel
        v-for="(sensor, idx) in sensorStore.sensors_power"
        :key="idx"
      >
        <accordion-header>{{
          sensor.name.length === 0 ? 'none' : sensor.name
        }}</accordion-header>
        <accordion-content>
          <div>
            <SensorElement :selected_sensor_index="idx" />
          </div>
        </accordion-content>
      </accordion-panel>
    </Accordion>
  </div>
</template>

<script setup lang="ts">
import {
  Accordion,
  AccordionPanel,
  AccordionHeader,
  AccordionContent,
} from 'flowbite-vue';
import { useSensorStore } from '../../stores/SensorStore';
import SensorElement from '../Elements/Sensors/PowerSensorElement.vue';

const sensorStore = useSensorStore();
</script>
