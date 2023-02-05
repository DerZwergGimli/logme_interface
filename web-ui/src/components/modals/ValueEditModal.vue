<script setup lang="ts">
import { Modal } from 'flowbite-vue';
import { Button } from 'flowbite-vue';
import { onBeforeMount, onMounted, ref } from 'vue';
import { createToast } from 'mosha-vue-toastify';
import { TOAST_ERROR, TOAST_INFO } from '../../scripts/toast_config';
import { useSensorStore } from '../../stores/SensorStore.js';
import EditIcon from '../icons/EditIcon.vue';

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
    .then(response => response.json())
    .then(response => {
      console.log(response);
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
  <Modal :size="props.size" v-if="props.isShown" @close="$emit('closeModal')">
    <template #header>
      <div class="flex flex-row items-center space-x-2">
        <edit-icon class="w-9" />
        <h2 class="text-lg">
          Editing Sensor[{{ props.index_to_edit }}] Config
        </h2>
      </div>
    </template>
    <template #body>
      <div class="grid grid-cols-3 gap-3">
        <p>ID:</p>
        <Input
          type="number"
          class="col-span-2"
          placeholder="enter new value here"
          :value="useSensorStore().sensors[props.index_to_edit].id"
          @input="
            (event: any) =>
              (useSensorStore().sensors[props.index_to_edit].id =
                event.target.value)
          "
        />
        <p>Name:</p>
        <Input
          type="text"
          class="col-span-2"
          placeholder="enter new value here"
          :value="useSensorStore().sensors[props.index_to_edit].name"
          @input="
             (event: any) =>
              (useSensorStore().sensors[props.index_to_edit].name =
                event.target.value)
          "
        />
        <p>Description:</p>
        <Input
          type="text"
          class="col-span-2"
          placeholder="enter new value here"
          :value="useSensorStore().sensors[props.index_to_edit].description"
          @input="
            (event: any) =>
              (useSensorStore().sensors[props.index_to_edit].description =
                event.target.value)
          "
        />
        <p>Pin [RX]:</p>
        <Input
          type="number"
          class="col-span-2"
          placeholder="enter new value here"
          :value="useSensorStore().sensors[props.index_to_edit].pin_rx"
          @input="
             (event: any) =>
              (useSensorStore().sensors[props.index_to_edit].pin_rx =
                event.target.value)
          "
        />
        <p>Pin [TX]:</p>
        <Input
          type="number"
          class="col-span-2"
          placeholder="enter new value here"
          :value="useSensorStore().sensors[props.index_to_edit].pin_tx"
          @input="
            (event: any) =>
              (useSensorStore().sensors[props.index_to_edit].pin_tx =
                event.target.value)
          "
        />
        <p>Baudrate:</p>
        <Input
          class="col-span-2"
          type="number"
          placeholder="enter new value here"
          :value="useSensorStore().sensors[props.index_to_edit].baudrate"
          @input="
             (event: any) =>
              (useSensorStore().sensors[props.index_to_edit].baudrate =
                event.target.value)
          "
        />
        <p>Primary-Address:</p>
        <Input
          class="col-span-2"
          type="number"
          placeholder="enter new value here"
          :value="useSensorStore().sensors[props.index_to_edit].primary_address"
          @input="
            (event: any) =>
              (useSensorStore().sensors[props.index_to_edit].primary_address =
                event.target.value)
          "
        />
        <p>Secondary-Address:</p>
        <Input
          class="col-span-2"
          type="number"
          placeholder="enter new value here"
          :value="
            useSensorStore().sensors[props.index_to_edit].secondary_address
          "
          @input="
            (event: any) =>
              (useSensorStore().sensors[props.index_to_edit].secondary_address =
                event.target.value)
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
