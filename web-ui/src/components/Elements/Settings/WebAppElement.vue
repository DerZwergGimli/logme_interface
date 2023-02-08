<script setup lang="ts">
import { ref } from 'vue';

const app_version = ref(APP_VERSION);
const app_mode = ref(import.meta.env.MODE);
const app_url = ref(APP_API_URL === '' ? '/' : APP_API_URL);
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
    </tbody>
  </table>
</template>
