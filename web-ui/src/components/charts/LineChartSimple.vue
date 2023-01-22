<template>
  <Line ref="lineChart" :options="chartOptions" :data="chartData" />
</template>

<script setup lang="ts">
import { Line } from 'vue-chartjs';
import {
  Chart as ChartJS,
  CategoryScale,
  LinearScale,
  PointElement,
  LineElement,
  Title,
  Tooltip,
  Legend,
} from 'chart.js';
import {computed, onMounted, reactive, ref} from 'vue';

ChartJS.register(
  CategoryScale,
  LinearScale,
  PointElement,
  LineElement,
  Title,
  Tooltip
);

const chartData = computed(() => {
  return {
    labels: [1,2],
        datasets: [{ backgroundColor: '#f87979', data: [2,3] }],
  }
})



const chartOptions = {
  responsive: true,
};

onMounted(() => {
  chartData.value.labels.push(3 as never);
  chartData.value.datasets[0].data.push(10 as never);

  chartData.value.labels.push(4 as never);
  chartData.value.datasets[0].data.push(10 as never);

});
</script>
