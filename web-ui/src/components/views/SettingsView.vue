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
        <accordion-header>Network</accordion-header>
        <accordion-content>
          <network-element />
        </accordion-content>
      </accordion-panel>
      <accordion-panel>
        <accordion-header>Sensor-Store</accordion-header>
        <accordion-content>
          <sensor-type-toggle />
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
<script setup lang="ts">
import NetworkElement from '../Elements/Settings/NetworkElement.vue';
import SystemInfoElement from '../Elements/Settings/SystemInfoElement.vue';
import WebAppElement from '../Elements/Settings/WebAppElement.vue';
import { onMounted } from 'vue';
import { createToast } from 'mosha-vue-toastify';
import { TOAST_INFO } from '../../scripts/toast_config';
import {
  Accordion,
  AccordionPanel,
  AccordionHeader,
  AccordionContent,
} from 'flowbite-vue';
import SensorStoreSettingsTable from '../tables/SensorStoreSettingsTable.vue';
import SensorTypeToggle from '../buttons/SensorTypeToggle.vue';
</script>
