<script setup lang="ts">
import { Modal, Button } from 'flowbite-vue';
import { createToast } from 'mosha-vue-toastify';
import { TOAST_ERROR, TOAST_INFO } from '../../scripts/toast_config';
import { useSensorStore } from '../../stores/SensorStore.js';
import EditIcon from '../icons/EditIcon.vue';
import { useAppStore } from '../../stores/AppStore.js';
import ToggleButton from '../buttons/ToggleButton.vue';

const props = defineProps(['size', 'isShown']);
const emit = defineEmits(['closeModal']);

const options = [
  { text: 'ws', value: 'ws' },
  { text: 'wss', value: 'wss' },
  { text: 'mqtt', value: 'mqtt' },
  { text: 'mqtts', value: 'mqtts' },
];

function send_mqtt_config() {
  fetch(APP_API_URL + '/mqtt/edit', {
    method: 'POST',
    headers: {
      Accept: 'application/json',
      'Content-Type': 'application/json',
    },
    body: JSON.stringify(useAppStore().mqtt),
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
        <h2 class="text-lg">Editing Sensor Config</h2>
      </div>
    </template>
    <template #body>
      <div class="grid grid-cols-3 gap-3 items-center">
        <p>Protocol:</p>
        <div class="col-span-2">
          <select
            v-model="useAppStore().mqtt.protocol"
            id="countries"
            class="bg-gray-50 border border-gray-300 text-gray-900 text-sm rounded-lg focus:ring-blue-500 focus:border-blue-500 block w-full p-2.5 dark:bg-gray-700 dark:border-gray-600 dark:placeholder-gray-400 dark:text-white dark:focus:ring-blue-500 dark:focus:border-blue-500"
          >
            <option v-for="option in options" v-bind:value="option.value">
              {{ option.text }}
            </option>
          </select>
        </div>
        <p>IP:</p>
        <Input
          v-bind:value="useAppStore().mqtt.ip"
          @input="
            (event: any) => {
              useAppStore().mqtt.ip = event.target.value;
            }
          "
          type="text"
          class="col-span-2"
          placeholder="enter IP here"
        />
        <p>Port:</p>
        <Input
          v-bind:value="useAppStore().mqtt.port"
          @input="
            (event: any) => {
              useAppStore().mqtt.port = event.target.value;
            }
          "
          type="number"
          class="col-span-2"
          placeholder="enter IP here"
        />
        <p>Username:</p>
        <Input
          v-bind:value="useAppStore().mqtt.username"
          @input="
            (event: any) => {
              useAppStore().mqtt.username = event.target.value;
            }
          "
          type="text"
          class="col-span-2"
          placeholder="enter IP here"
        />
        <p>Password:</p>
        <Input
          v-bind:value="useAppStore().mqtt.password"
          @input="
            (event: any) => {
              useAppStore().mqtt.password = event.target.value;
            }
          "
          type="text"
          class="col-span-2"
          placeholder="enter IP here"
        />
        <p>enabled:</p>
        <toggle-button
          v-bind:is_checked="useAppStore().mqtt.enabled"
          @click="useAppStore().mqtt.enabled = !useAppStore().mqtt.enabled"
        />
      </div>
    </template>
    <template #footer>
      <div class="flex justify-between">
        <Button @click="$emit('closeModal')" color="red">Cancel</Button>
        <Button @click="send_mqtt_config()" color="default">Save</Button>
      </div>
    </template>
  </Modal>
</template>
