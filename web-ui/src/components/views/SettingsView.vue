<template>
  <div class="flex flex-col m-3">
    <Accordion>
      <accordion-panel>
        <accordion-header>System</accordion-header>
        <accordion-content>
          <div>
            <system-info-element />
          </div>
        </accordion-content>
      </accordion-panel>
      <accordion-panel>
        <accordion-header>Network</accordion-header>
        <accordion-content>
          <div>
            <network-element />
          </div>
        </accordion-content>
      </accordion-panel>
      <accordion-panel>
        <accordion-header>App</accordion-header>
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

onMounted(async () => {
  createToast('Loading data...', TOAST_INFO);
});
</script>
