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
        <th></th>
      </tr>
    </thead>
    <tbody>
      <tr v-for="cron_job in useAppStore().system?.cron_jobs" :key="cron_job">
        <td>{{ cron_job.job_name }}</td>
        <td>{{ cron_job.job_description }}</td>
        <td>{{ cron_job.job_schedule }}</td>
        <td><Button>EDIT</Button></td>
      </tr>
    </tbody>
  </table>
</template>
