<script setup lang="ts">
import { useAppStore } from '../../../stores/AppStore';
import { Button } from 'flowbite-vue';
</script>
<template>
  <table id="tableTABLE">
    <thead id="tableHEAD">
      <tr>
        <th>Name</th>
        <th>Description</th>
        <th>Schedule</th>
        <th>Enabled</th>
        <th></th>
      </tr>
    </thead>
    <tbody>
      <tr v-for="(cron_job, idx) in useAppStore().system?.cron_jobs" :key="idx">
        <td>{{ cron_job.job_name }}</td>
        <td>{{ cron_job.job_description }}</td>
        <td>{{ cron_job.job_schedule }}</td>
        <td>{{ cron_job.enabled }}</td>
        <td class="float-right"><Button>EDIT</Button></td>
      </tr>
    </tbody>
  </table>
</template>
