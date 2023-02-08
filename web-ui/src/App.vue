<script setup lang="ts">
import HeaderComponent from './components/header_footer/HeaderComponent.vue';
import FooterComponent from './components/header_footer/FooterComponent.vue';
import { useSensorStore } from './stores/SensorStore';
import { createToast } from 'mosha-vue-toastify';
import { TOAST_ERROR } from './scripts/toast_config';
import { useAppStore } from './stores/AppStore';

const appStore = useAppStore();
const sensorStore = useSensorStore();

appStore.init();
appStore.fetch_wifi();

setInterval(() => {
  fetch_endpoints_async();
}, 3000);

async function fetch_endpoints_async() {
  await fetch(APP_API_URL + '/sensors')
    .then(resp => resp.json())
    .then(json => {
      useSensorStore().sensors = json;
    })
    .catch(() => {
      createToast('Error fetching /sensors', TOAST_ERROR);
      return;
    });

  await fetch(APP_API_URL + '/system')
    .then(resp => resp.json())
    .then(json => {
      useAppStore().system = json;
    })
    .catch(() => {
      createToast('Error fetching /system', TOAST_ERROR);
      return;
    });
}
</script>

<template>
  <div class="flex flex-col justify-between min-h-screen">
    <header-component
      class="border-b border-black dark:border-white mb-3 sticky top-0 z-50"
    />
    <router-view class="mb-auto pb-3 my-3 sm:mx-2"></router-view>
    <footer-component
      class="w-full bottom-0 border-t border-black dark:border-white"
    />
  </div>
</template>
