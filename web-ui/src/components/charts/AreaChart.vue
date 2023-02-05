<script lang="ts" setup>
import { computed, getCurrentInstance, ref, watch } from 'vue';

const props = defineProps({
  chart_heading: {
    type: String,
    default: 'SomeText',
  },
  time_values: {
    type: Array,
    default: [],
  },
  data_values: {
    type: Array,
    default: [],
  },
  unit_text: {
    type: String,
    default: '-',
  },
  is_dark: {
    type: Boolean,
    default: false,
  },
});

const chartOptions = computed(() => {
  return {
    chart: {
      foreColor: props.is_dark ? '#fff' : '#000',
      id: 'line-chart',
      type: 'line',
      animations: {
        enabled: true,
        easing: 'linear',
      },
      toolbar: {
        show: true,
      },
      zoom: {
        enabled: false,
      },
    },
    colors: ['#642ba9'],
    tooltip: {
      theme: props.is_dark ? 'dark' : 'light',
    },
    stroke: {
      curve: 'smooth',
    },
    dataLabels: {
      enabled: false,
    },
    xaxis: {
      type: 'datetime',
      categories: props.time_values,
    },
    title: {
      text: props.chart_heading,
      floating: true,
      align: 'center',
    },
  };
});

const series = ref([
  {
    name: 'series-1',
    data: props.data_values,
  },
]);

const timeFrame = ref(5);

// watch(props.time_values, () => {
//   if (props.time_values) {
//     while (
//       (props.time_values[0] as number) <
//       Date.now() - timeFrame.value * 1000
//     ) {
//       props.time_values.shift();
//       props.data_values.shift();
//     }
//   }
// });
</script>

<template>
  <div id="chart">
    <apexchart
      type="area"
      height="350"
      :options="chartOptions"
      :series="series"
    ></apexchart>
    <div class="flex flex-row px-3 pb-1">
      <div class="flex text-right w-full items-center dark:text-white">
        Timeframe:
      </div>
      <select
        id="timeFrameSelect"
        v-model="timeFrame"
        class="bg-gray-50 border border-gray-300 text-gray-900 text-sm rounded-lg focus:ring-blue-500 focus:border-blue-500 block p-2.5 dark:bg-gray-700 dark:border-gray-600 dark:placeholder-gray-400 dark:text-white dark:focus:ring-blue-500 dark:focus:border-blue-500"
      >
        <option selected :value="5">5sec</option>
        <option :value="60">1min</option>
        <option :value="5 * 60">5min</option>
        <option :value="60 * 60">1h</option>
      </select>
    </div>
  </div>
</template>
