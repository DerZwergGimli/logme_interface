<script setup lang="ts">
import { computed, onMounted, ref, watch } from 'vue';

const props = defineProps({
  chart_heading: {
    type: String,
    default: 'SomeText',
  },
  time_values: {
    type: Array,
  },
  data_values: {
    type: Array,
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

const series = ref([
  {
    name: 'value',
    data: props.data_values,
  },
]);

const chartOptions = computed(() => {
  return {
    chart: {
      foreColor: props.is_dark ? '#fff' : '#000',
      height: 350,
      type: 'bar',
      toolbar: {
        show: false,
      },
    },
    colors: ['#eac712'],
    fill: {
      type: 'gradient',
      gradient: {
        gradientToColors: ['#ff5141'],
        type: 'vertical',
      },
    },
    tooltip: {
      theme: props.is_dark ? 'dark' : 'light',
    },

    plotOptions: {
      bar: {
        borderRadius: 10,
        dataLabels: {
          position: 'top', // top, center, bottom
        },
      },
    },

    dataLabels: {
      enabled: false,
      formatter: function (val: any) {
        return val.toString() + ' ' + props.unit_text;
      },
      offsetY: -20,
      style: {
        fontSize: '12px',
        colors: [props.is_dark ? '#FFF' : '#000'],
      },
    },
    xaxis: {
      categories: props.time_values,
      position: 'bottom',
      type: 'datetime',
      axisBorder: {
        show: false,
      },
      axisTicks: {
        show: false,
      },
    },
    yaxis: {
      axisBorder: {
        show: false,
      },
      axisTicks: {
        show: true,
      },
      labels: {
        show: true,
        formatter: function (val: any) {
          return val.toString() + ' ' + props.unit_text;
        },
      },
    },
    title: {
      text: props.chart_heading,
      floating: true,
      align: 'center',
    },
  };
});
</script>
<template>
  <div id="chart">
    <apexchart
      type="bar"
      height="350"
      :options="chartOptions"
      :series="series"
    ></apexchart>
  </div>
</template>
