<script setup lang="ts">
import { Modal } from 'flowbite-vue';
import { Button } from 'flowbite-vue';
import { onBeforeMount, onMounted, ref } from 'vue';
import { createToast } from 'mosha-vue-toastify';
import { TOAST_ERROR, TOAST_INFO } from '../../scripts/toast_config';
import { useSensorStore } from '../../stores/SensorStore.js';

const props = defineProps([
  'index_to_edit',
  'endpoint_to_update',
  'size',
  'isShown',
]);
const emit = defineEmits(['closeModal']);

function send_sensor_config() {
  fetch(APP_API_URL + '/sensoredit/' + props.index_to_edit, {
    method: 'POST',
    headers: {
      Accept: 'application/json',
      'Content-Type': 'application/json',
    },
    body: JSON.stringify({
      id: useSensorStore().sensors[props.index_to_edit].id,
      name: useSensorStore().sensors[props.index_to_edit].name,
      description: useSensorStore().sensors[props.index_to_edit].description,
      pin_rx: useSensorStore().sensors[props.index_to_edit].pin_rx,
      pin_tx: useSensorStore().sensors[props.index_to_edit].pin_tx,
      baudrate: useSensorStore().sensors[props.index_to_edit].baudrate,
      primary_address:
        useSensorStore().sensors[props.index_to_edit].primary_address,
      secondary_address:
        useSensorStore().sensors[props.index_to_edit].secondary_address,
    }),
  })
    .then(response => response.text())
    .then(response => {
      console.log(response);
      createToast('Sensor config send!', TOAST_INFO);
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
  <Modal :size="props.size" v-if="props.isShown" @close="$emit('closeModal')">
    <template #header>
      <h2 class="flex items-center text-lg">
        Edit Sensor[{{ props.index_to_edit }}] Config
      </h2>
    </template>
    <template #body>
      <div class="grid grid-cols-3 gap-3">
        <p>ID:</p>
        <Input
          type="number"
          class="col-span-2"
          placeholder="enter new value here"
          :value="useSensorStore().sensors[props.index_to_edit].id"
        />
        <p>Name:</p>
        <Input
          type="text"
          class="col-span-2"
          placeholder="enter new value here"
          :value="useSensorStore().sensors[props.index_to_edit].name"
        />
        <p>Description:</p>
        <Input
          type="text"
          class="col-span-2"
          placeholder="enter new value here"
          :value="useSensorStore().sensors[props.index_to_edit].description"
        />
        <p>Pin [RX]:</p>
        <Input
          type="number"
          class="col-span-2"
          placeholder="enter new value here"
          :value="useSensorStore().sensors[props.index_to_edit].pin_rx"
        />
        <p>Pin [TX]:</p>
        <Input
          type="number"
          class="col-span-2"
          placeholder="enter new value here"
          :value="useSensorStore().sensors[props.index_to_edit].pin_tx"
        />
        <p>Baudrate:</p>
        <Input
          class="col-span-2"
          type="number"
          placeholder="enter new value here"
          :value="useSensorStore().sensors[props.index_to_edit].baudrate"
        />
        <p>Primary-Address:</p>
        <Input
          class="col-span-2"
          type="number"
          placeholder="enter new value here"
          :value="useSensorStore().sensors[props.index_to_edit].primary_address"
        />
        <p>Secondary-Address:</p>
        <Input
          class="col-span-2"
          type="number"
          placeholder="enter new value here"
          :value="
            useSensorStore().sensors[props.index_to_edit].secondary_address
          "
        />
      </div>
    </template>
    <template #footer>
      <div class="flex justify-between">
        <Button @click="$emit('closeModal')" color="red">Cancel</Button>
        <Button @click="send_sensor_config()" color="default">Save</Button>
      </div>
    </template>
  </Modal>
</template>
