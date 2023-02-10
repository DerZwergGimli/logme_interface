<script setup lang="ts">
import { useAppStore } from '../../../stores/AppStore';
import { Button } from 'flowbite-vue';
import { ref } from 'vue';
import CronJobEditModal from '../../modals/CronJobEditModal.vue';

const modalShown = ref(false);
const cron_index = ref(0);
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
      <tr v-for="(cron_job, idx) in useAppStore().cron_jobs" :key="idx">
        <td>{{ cron_job.name }}</td>
        <td>{{ cron_job.description }}</td>
        <td>{{ cron_job.schedule }}</td>
        <td>{{ cron_job.enabled }}</td>
        <td class="float-right">
          <Button
            @click="
              () => {
                modalShown = true;
                cron_index = idx;
              }
            "
            >EDIT</Button
          >
        </td>
      </tr>
    </tbody>
  </table>

  <div v-if="modalShown">
    <cron-job-edit-modal
      :is-shown="modalShown"
      :index_selected="cron_index"
      @closeModal="modalShown = false"
    />
  </div>
</template>
