<template>
  <div v-if="sensor" class="space-y-3">
    <div class="flex flex-row">
      <smart-meter-icon class="w-24 dark:text-gray-100"></smart-meter-icon>
      <div class="w-full grid grid-cols-2">
        <div class="grid grid-cols-1">
          <value-element
            :value="sensor.id.toString()"
            icon="id"
            description="SensorID"
            unit=""
            :editable="false"
          />
          <value-element
            :value="sensor.name.length === 0 ? 'none' : sensor.name"
            icon="info"
            description="SensorName"
            unit=""
            @buttonClick="action_edit(sensor.name)"
          />
          <value-element
            :value="sensor.description"
            icon="info"
            description="Description"
            unit=""
            @buttonClick="action_edit(sensor.name)"
          />
        </div>

        <div class="grid grid-cols-1">
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
    </div>

    <area-chart
      chart_heading="Consumption"
      class="border-2 p-1"
      :data_values="sensor.chart_data"
      :time_values="sensor.chart_timestamps"
      :is_dark="!useAppStore().themeIsDark"
    />
    <BarChart
      class="border-2 p-1"
      chart_heading="Daily"
      :time_values="time_values_hourly ?? []"
      :data_values="sensor.history.day_24_kw"
      unit_text="kWh"
      :is_dark="!useAppStore().themeIsDark"
    />
    <BarChart
      class="border-2 p-1"
      chart_heading="Weekly"
      :time_values="time_values_weekly"
      :data_values="sensor.history.week_7_kw"
      unit_text="kWh"
      :is_dark="!useAppStore().themeIsDark"
    />
    <BarChart
      class="border-2 p-1"
      chart_heading="Monthly"
      :time_values="time_values_monthly"
      :data_values="sensor.history.month_30_kw"
      unit_text="kWh"
      :is_dark="!useAppStore().themeIsDark"
    />
  </div>
  <value-edit-modal
    :is-shown="isShowModal"
    @closeModal="isShowModal = false"
    :value_to_edit="value_to_edit"
  ></value-edit-modal>
</template>

<script setup lang="ts">
import ValueElement from './ValueElement.vue';
import AreaChart from '../../charts/AreaChart.vue';
import { Ref, ref } from 'vue';
import { useAppStore } from '../../../stores/AppStore.js';
import { useSensorStore } from '../../../stores/SensorStore.js';
import BarChart from '../../charts/BarChart.vue';
import SmartMeterIcon from '../../icons/SmartMeterIcon.vue';
import ValueEditModal from '../../modals/ValueEditModal.vue';

const isShowModal = ref(false);
const value_to_edit = ref();

const time_values_hourly = ref([]);
for (let i = 24; i > 0; i--) {
  time_values_hourly.value.push((Date.now() - i * 60 * 60 * 1000) as never);
}

const time_values_weekly = ref([]);
for (let i = 7; i > 0; i--) {
  time_values_weekly.value.push(
    (Date.now() - i * 24 * 60 * 60 * 1000) as never
  );
}

const time_values_monthly = ref([]);
for (let i = 30; i > 0; i--) {
  time_values_monthly.value.push(
    (Date.now() - i * 24 * 60 * 60 * 1000) as never
  );
}

const props = defineProps({
  selected_sensor_index: {
    type: Number,
    default: -1,
  },
});

function action_edit(value: string) {
  isShowModal.value = true;
  value_to_edit.value = value;
}

const sensor = ref(useSensorStore().sensors_power[props.selected_sensor_index]);
</script>
