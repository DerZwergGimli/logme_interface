<template>
  <div class="animate-fade-in flex flex-col" id="glowElement">
    <Accordion>
      <accordion-panel
        v-for="(sensor, idx) in useSensorStore().sensors"
        :key="idx"
      >
        <accordion-header>
          <div class="flex flex-row items-center pr-3 space-x-3">
            <div class="w-full">
              {{ sensor.name.length === 0 ? 'none' : sensor.name }}
            </div>
            <status-element
              class="animate-pulse animate-duration-2s"
              :status="sensor.status"
            />
            <Button @click="action_edit(idx)">
              <settings-icon class="w-6"
            /></Button></div
        ></accordion-header>
        <accordion-content>
          <div>
            <SensorElement :selected_sensor_index="idx" />
          </div>
        </accordion-content>
      </accordion-panel>
    </Accordion>
    <value-edit-modal
      :is-shown="isShowModal"
      @closeModal="isShowModal = false"
      :index_to_edit="sensor_index_to_edit"
    ></value-edit-modal>
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
import ValueEditModal from '../modals/ValueEditModal.vue';
import { ref } from 'vue';
import StatusElement from '../Elements/Sensors/StatusElement.vue';

const isShowModal = ref(false);
const sensor_index_to_edit = ref();

function action_edit(value: number) {
  isShowModal.value = true;
  sensor_index_to_edit.value = value;
}
</script>
