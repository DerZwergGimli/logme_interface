<template>
  <div class="flex flex-col" id="glowElement">
    <Accordion>
      <accordion-panel
        v-for="(sensor, idx) in useSensorStore().sensors"
        :key="idx"
      >
        <accordion-header>
          <div class="flex flex-row items-center pr-3">
            <div class="w-full">
              {{ sensor.name.length === 0 ? 'none' : sensor.name }}
            </div>
            <Button> <settings-icon class="w-6" /></Button></div
        ></accordion-header>
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
  Button,
} from 'flowbite-vue';
import { useSensorStore } from '../../stores/SensorStore';
import SensorElement from '../Elements/Sensors/PowerSensorElement.vue';
import SettingsIcon from '../icons/SettingsIcon.vue';
</script>
