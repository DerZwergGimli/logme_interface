<script setup lang="ts">
import NetworkElement from '../Elements/Settings/NetworkElement.vue';
import SystemInfoElement from '../Elements/Settings/SystemInfoElement.vue';
import WebAppElement from '../Elements/Settings/WebAppElement.vue';

import {
  Accordion,
  AccordionPanel,
  AccordionHeader,
  AccordionContent,
} from 'flowbite-vue';
import SensorStoreSettingsTable from '../tables/SensorStoreSettingsTable.vue';
import CronJobsElement from '../Elements/Settings/CronJobsElement.vue';
import MqttElement from '../Elements/Settings/MqttElement.vue';
</script>

<template>
  <div class="animate-fade-in flex flex-col">
    <Accordion
      flush
      class="border-2 rounded-xl dark:border-gray-700"
      id="glowElement"
    >
      <accordion-panel>
        <accordion-header>System</accordion-header>
        <accordion-content>
          <system-info-element />
        </accordion-content>
      </accordion-panel>
      <accordion-panel>
        <accordion-header>CronJobs</accordion-header>
        <accordion-content>
          <cron-jobs-element />
        </accordion-content>
      </accordion-panel>
      <accordion-panel>
        <accordion-header>MQTT</accordion-header>
        <accordion-content>
          <mqtt-element />
        </accordion-content>
      </accordion-panel>
      <accordion-panel>
        <accordion-header>Network</accordion-header>
        <accordion-content>
          <network-element />
        </accordion-content>
      </accordion-panel>
      <accordion-panel>
        <accordion-header>Sensor-Store</accordion-header>
        <accordion-content>
          <sensor-store-settings-table />
        </accordion-content>
      </accordion-panel>
      <accordion-panel>
        <accordion-header>WebUI</accordion-header>
        <accordion-content>
          <web-app-element />
        </accordion-content>
      </accordion-panel>
    </Accordion>
  </div>
</template>
