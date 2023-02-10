<script setup lang="ts">
import { ref } from 'vue';
import ToggleButton from '../../buttons/ToggleButton.vue';
import { useAppStore } from '../../../stores/AppStore.js';

const app_version = ref(APP_VERSION);
const app_mode = ref(import.meta.env.MODE);
const app_url = ref(APP_API_URL === '' ? '/' : APP_API_URL);
const options = [
  { text: '1s', value: 1000 },
  { text: '3s', value: 1000 * 3 },
  { text: '5s', value: 1000 * 5 },
  { text: '10s', value: 1000 * 10 },
  { text: '30s', value: 1000 * 30 },
  { text: '60s', value: 1000 * 60 },
];
</script>
<template>
  <table id="tableTABLE">
    <thead id="tableHEAD">
      <tr>
        <th>Element</th>
        <th>Value</th>
      </tr>
    </thead>
    <tbody>
      <tr>
        <td>Version</td>
        <td>{{ app_version }}</td>
      </tr>
      <tr>
        <td>Mode</td>
        <td>{{ app_mode }}</td>
      </tr>
      <tr>
        <td>API-URL</td>
        <td>{{ app_url }}</td>
      </tr>
      <tr>
        <td>Show heap</td>
        <td>
          <toggle-button
            @click="useAppStore().toggle_showHeap()"
            :is_checked="useAppStore().showHeap"
          />
        </td>
      </tr>
      <tr>
        <td>UI-Update-Interval</td>
        <td>
          <select
            v-model="useAppStore().update_interval"
            id="countries"
            class="bg-gray-50 border border-gray-300 text-gray-900 text-sm rounded-lg focus:ring-blue-500 focus:border-blue-500 block w-full p-2.5 dark:bg-gray-700 dark:border-gray-600 dark:placeholder-gray-400 dark:text-white dark:focus:ring-blue-500 dark:focus:border-blue-500"
          >
            <option v-for="option in options" v-bind:value="option.value">
              {{ option.text }}
            </option>
          </select>
        </td>
      </tr>
    </tbody>
  </table>
</template>
