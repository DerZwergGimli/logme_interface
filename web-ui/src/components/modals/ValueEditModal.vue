<script setup lang="ts">
import { Modal } from 'flowbite-vue';
import { Button } from 'flowbite-vue';
import { onBeforeMount, onMounted, ref } from 'vue';
import { createToast } from 'mosha-vue-toastify';
import { TOAST_ERROR, TOAST_INFO } from '../../scripts/toast_config';

const props = defineProps([
  'value_to_edit',
  'endpoint_to_update',
  'size',
  'isShown',
]);
const emit = defineEmits(['closeModal']);

// const isShowModal = ref(false);
// function closeModal() {
//   isShowModal.value = false;
// }
// function showModal() {
//   isShowModal.value = true;
// }

function send_wifi_config() {
  fetch(APP_API_URL + '/edit_name/i', {
    method: 'POST',
    headers: {
      Accept: 'application/json',
      'Content-Type': 'application/json',
    },
    body: JSON.stringify({}),
  })
    .then(response => response.text())
    .then(response => {
      console.log(response);
      createToast('New Wifi-Config send!', TOAST_INFO);
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
      <h2 class="flex items-center text-lg">Edit</h2>
    </template>
    <template #body>
      <div class="flex flex-col space-y-5">
        <Input
          class="basis-auto"
          placeholder="enter new value here"
          @input=""
          :value="props.value_to_edit"
        />
      </div>
    </template>
    <template #footer>
      <div class="flex justify-between">
        <Button @click="$emit('closeModal')" color="red">Cancel</Button>
        <Button @click="send_wifi_config()" color="default">Save</Button>
      </div>
    </template>
  </Modal>
</template>
