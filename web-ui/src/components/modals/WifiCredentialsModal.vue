<script setup lang="ts">
import { Modal } from 'flowbite-vue';
import { Button } from 'flowbite-vue';
import { onBeforeMount, onMounted, ref } from 'vue';
import { createToast } from 'mosha-vue-toastify';
import { TOAST_ERROR, TOAST_INFO } from '../../scripts/toast_config';

const pros = defineProps(['ssid', 'isShown', 'size']);
const emit = defineEmits(['closeModal']);

const network_ssid_user = ref();
const network_password_user = ref();

network_ssid_user.value = pros.ssid;

const isShowModal = ref(false);
function closeModal() {
  isShowModal.value = false;
}
function showModal() {
  isShowModal.value = true;
}

function send_wifi_config() {
  fetch(APP_API_URL + '/wificonfig', {
    method: 'POST',
    headers: {
      Accept: 'application/json',
      'Content-Type': 'application/json',
    },
    body: JSON.stringify({
      ssid: network_ssid_user.value,
      password: network_password_user.value,
    }),
  })
    .then(response => response.json())
    .then(response => {
      console.log(JSON.stringify(response));
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
  <Modal :size="pros.size" v-if="pros.isShown" @close="closeModal">
    <template #header>
      <h2 class="flex items-center text-lg">Wifi Setup</h2>
      {{ pros.ssid }}
      {{ network_ssid_user }}
    </template>
    <template #body>
      <div class="flex flex-col space-y-5">
        <p>You are about to connect to a new AccessPoint.</p>
        <div class="flex flex-col space-y-2">
          <div class="flex flex-row space-x-3">
            <p class="basis-1/4">SSID:</p>
            <Input
              class="basis-auto"
              placeholder="enter ssid here"
              @input="(event: any) => (network_ssid_user = event.target.value)"
              :value="pros.ssid"
            />
          </div>
          <div class="flex flex-row space-x-3">
            <p class="basis-1/4">Password:</p>
            <Input
              class="basis-auto"
              placeholder="enter password here"
              @input="(event: any) => (network_password_user = event.target.value)"
              :value="network_password_user"
            />
          </div>
        </div>
        <p class="shadow-lg bg-yellow-700 rounded-md p-1">
          After clicking on 'Connect' the Device will try to connect to the new
          AccessPoint!
        </p>
      </div>
    </template>
    <template #footer>
      <div class="flex justify-between">
        <Button @click="$emit('closeModal')" color="red">Cancel</Button>
        <Button @click="send_wifi_config()" color="default">Connect</Button>
      </div>
    </template>
  </Modal>
</template>
