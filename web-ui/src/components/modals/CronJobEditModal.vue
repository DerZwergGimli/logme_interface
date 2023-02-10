<script setup lang="ts">
import { Modal, Button } from 'flowbite-vue';
import { createToast } from 'mosha-vue-toastify';
import { TOAST_ERROR, TOAST_INFO } from '../../scripts/toast_config';
import EditIcon from '../icons/EditIcon.vue';
import { useAppStore } from '../../stores/AppStore.js';
import ToggleButton from '../buttons/ToggleButton.vue';

const props = defineProps(['size', 'isShown', 'index_selected']);
const emit = defineEmits(['closeModal']);

const options = [
  { text: 'ws', value: 'ws' },
  { text: 'wss', value: 'wss' },
  { text: 'mqtt', value: 'mqtt' },
  { text: 'mqtts', value: 'mqtts' },
];

function send_cron_config() {
  fetch(APP_API_URL + '/cron/edit', {
    method: 'POST',
    headers: {
      Accept: 'application/json',
      'Content-Type': 'application/json',
    },
    body: JSON.stringify(useAppStore().cron_jobs),
  })
    .then(response => response.json())
    .then(response => {
      createToast(response.message, TOAST_INFO);
    })
    .catch(err => {
      console.error(err);
      createToast('Error sending new config!', TOAST_ERROR);
    })
    .finally(() => {
      emit('closeModal');
    });
}
</script>

<template>
  <Modal
    id="modalElement"
    :size="props.size"
    v-if="props.isShown"
    @close="$emit('closeModal')"
  >
    <template #header>
      <div class="flex flex-row items-center space-x-2">
        <edit-icon class="w-9" />
        <h2 class="text-lg">Editing CronJob Config</h2>
      </div>
    </template>
    <template #body>
      <div class="grid grid-cols-3 gap-3 items-center">
        <p>Name:</p>
        <div class="col-span-2">
          <p>{{ useAppStore().cron_jobs[index_selected].name }}</p>
        </div>
        <p>Description:</p>
        <div class="col-span-2">
          <p>{{ useAppStore().cron_jobs[index_selected].description }}</p>
        </div>
        <p>Schedule:</p>
        <Input
          v-bind:value="useAppStore().cron_jobs[index_selected].schedule"
          @input="
            (event: any) => {
              useAppStore().cron_jobs[index_selected].schedule = event.target.value;
            }
          "
          type="text"
          class="col-span-2"
          placeholder="enter IP here"
        />

        <p>enabled:</p>
        <toggle-button
          v-bind:is_checked="useAppStore().cron_jobs[index_selected].enabled"
          @click="
            useAppStore().cron_jobs[index_selected].enabled =
              !useAppStore().cron_jobs[index_selected].enabled
          "
        />
      </div>
    </template>
    <template #footer>
      <div class="flex justify-between">
        <Button @click="$emit('closeModal')" color="red">Cancel</Button>
        <Button @click="send_cron_config()" color="default">Save</Button>
      </div>
    </template>
  </Modal>
</template>
