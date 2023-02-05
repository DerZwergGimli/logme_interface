<template>
  <div class="mx-3 flex flex-col space-y-3">
    <div
      id="glowElement"
      v-for="(sensor, idx) in useSensorStore().sensors"
      :key="idx"
      class=""
    >
      <div
        class="flex flex-row border-2 rounded-lg p-2 bg-gradient-to-r from-cyan-400 to-blue-400 dark:from-cyan-800 dark:to-blue-800"
      >
        <div class="flex flex-row basis-1/2 items-center justify-center">
          <smart-meter-icon class="w-24 dark:text-white" />

          <div class="flex flex-col w-full">
            <p class="flex w-full text-2xl">
              {{ sensor.name.length > 0 ? sensor.name : '---' }}
            </p>
            <p class="flex w-full text-md">
              {{ sensor.description.length > 0 ? sensor.description : '---' }}
            </p>
            <p class="flex w-full text-md">
              {{
                sensor.data?.salve_info !== undefined
                  ? sensor.data?.salve_info['manufacturer:']
                  : '---'
              }}
              {{
                sensor.data?.salve_info !== undefined
                  ? sensor.data?.salve_info['id:']
                  : '---'
              }}
            </p>
          </div>
        </div>
        <div class="flex flex-col basis-1/2 justify-center space-y-2">
          <div
            v-for="sensor_element_id in sensor.web_config_ids
              ? sensor.web_config_ids[0]
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
          <no-data-element v-if="sensor.data?.salve_info === undefined" />
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
import NoDataElement from '../Elements/Sensors/NoDataElement.vue';

const sensorStore = useSensorStore();
</script>
