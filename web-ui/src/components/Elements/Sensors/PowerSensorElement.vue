<template>
  <div v-if="sensor" class="space-y-3">
    <div class="flex flex-row space-x-4">
      <div
        class="flex-col items-center space-y-3 bg-gray-100 dark:bg-gray-800 p-2 rounded-lg"
      >
        <div class="flex justify-center">
          <smart-meter-icon class="w-24 dark:text-gray-100"></smart-meter-icon>
        </div>
        <div class="grid grid-cols-2 gap-2">
          <h4>Name:</h4>
          <h3>{{ sensor.name }}</h3>
          <h4>Status:</h4>
          <h3>{{ sensorstatus_toString(sensor.status) }}</h3>
          <h4>Description:</h4>
          <h4>{{ sensor.description }}</h4>
        </div>
      </div>

      <div class="w-full grid grid-cols-2 gap-2">
        <div
          v-for="sensor_element_id in sensor.web_config_ids
            ? sensor.web_config_ids[1]
            : []"
          :key="sensor_element_id"
        >
          <value-element
            :value="
              sensor.data?.slave_data.find(
                data => data.id === sensor_element_id
              )?.value
            "
            icon="numbers"
            :description="
              sensor.data?.slave_data
                .find(data => data.id === sensor_element_id)
                ?.unit?.split('(')[0]
            "
            :unit="
              sensor.data?.slave_data
                .find(data => data.id === sensor_element_id)
                ?.unit?.split('(')[1]
            "
            :editable="false"
          />
        </div>
        <no-data-element
          class="col-span-2"
          v-if="sensor.data?.slave_data === undefined"
        />
      </div>
    </div>

    <area-chart
      chart_heading="Consumption"
      class="border-4 rounded-md shadow-inner dark:border-gray-800"
      :data_values="sensor.live_data?.chart_data"
      :time_values="sensor.live_data?.chart_timestamps"
      :is_dark="!useAppStore().themeIsDark"
    />
    <BarChart
      class="border- p-1"
      chart_heading="Daily"
      :time_values="time_values_hourly ?? []"
      :data_values="sensor.history?.day_24_kw"
      unit_text="kWh"
      :is_dark="!useAppStore().themeIsDark"
    />
    <BarChart
      class="border-2 p-1"
      chart_heading="Weekly"
      :time_values="time_values_weekly"
      :data_values="sensor.history?.week_7_kw"
      unit_text="kWh"
      :is_dark="!useAppStore().themeIsDark"
    />
    <BarChart
      class="border-2 p-1"
      chart_heading="Monthly"
      :time_values="time_values_monthly"
      :data_values="sensor.history?.month_30_kw"
      unit_text="kWh"
      :is_dark="!useAppStore().themeIsDark"
    />
  </div>
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
import NoDataElement from './NoDataElement.vue';
import StatusElement from './StatusElement.vue';
import { sensorstatus_toString } from '../../../stores/interfaces/I_Sensor';

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

const sensor = ref(useSensorStore().sensors[props.selected_sensor_index]);
</script>
