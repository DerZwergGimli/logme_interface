<template>
  <div class="mx-3 flex flex-col space-y-3">
    <div
      v-for="(sensor, idx) in useSensorStore().sensors"
      v-if="useSensorStore().sensors"
      id="glowElement"
      :key="idx"
      class="animate-fade-in"
    >
      <div
        class="flex flex-col md:flex-row space-y-3 border-2 rounded-lg p-2 bg-gradient-to-r from-cyan-400 to-blue-400 dark:from-cyan-800 dark:to-blue-800"
      >
        <div class="flex flex-row basis-1/2 items-center justify-center">
          <smart-meter-icon
            class="w-24 dark:text-white animate-pulse animate-duration-2s"
          />

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
                  ? sensor.data?.salve_info.manufacturer
                  : '---'
              }}
              {{
                sensor.data?.salve_info !== undefined
                  ? sensor.data?.salve_info.id
                  : '---'
              }}
            </p>
          </div>
        </div>
        <div class="flex flex-col basis-1/2 justify-center space-y-2">
          <no-data-element v-if="sensor.data?.slave_data === undefined" />
          <div v-else  class="flex flex-col space-y-2">
            <div
              v-for="(datapoint, idx) in sensor.data?.slave_data.filter(data => sensor.dashboard_config_ids[0].some(cid => cid === data.id))" :key="idx"
            >
              <value-element :value="datapoint.value.toString()" :unit="datapoint.unit.toString()" :description="datapoint.quantity.toString()"></value-element>
            </div>
          </div>
        </div>
      </div>
    </div>

    <div v-if="useSensorStore().sensors.length === 0" class="flex p-5 w-full justify-center items-center">
      <spinner color="blue" size="12" />
    </div>
  </div>
</template>

<script lang="ts" setup>
import { useSensorStore } from '../../stores/SensorStore.js';
import SmartMeterIcon from '../icons/SmartMeterIcon.vue';
import { Spinner } from 'flowbite-vue';
import NoDataElement from '../Elements/Sensors/NoDataElement.vue';
import ValueElement from "../Elements/Sensors/ValueElement.vue";
import { I_SalveData_Function } from "../../stores/interfaces/I_Sensor.js";
</script>
