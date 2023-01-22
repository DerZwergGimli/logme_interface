<template>
  <div class="flex flex-col ">
    <Accordion>
      <accordion-panel
        v-for="(sensor, idx) in sensorStore.sensors"
        :key="idx"
      >
        <accordion-header>{{ sensor.name }}</accordion-header>
        <accordion-content>
          <div>
            <SensorElement :sensor="sensor" />
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
import SensorElement from '../Elements/Sensors/SensorElement.vue';

const sensorStore = useSensorStore()
</script>
