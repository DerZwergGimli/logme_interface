<script setup lang="ts">
import HeaderComponent from './components/header_footer/HeaderComponent.vue';
import FooterComponent from './components/header_footer/FooterComponent.vue';
import { useSensorStore } from './stores/SensorStore';
import { useAppStore } from './stores/AppStore';


const appStore = useAppStore();
const sensorStore = useSensorStore();

appStore.init();
appStore.fetch_wifi();
appStore.fetch_default_system_config();

setInterval(() => {
  fetch_endpoints_async();
}, appStore.update_interval);

async function fetch_endpoints_async() {

  await fetch(APP_API_URL + '/sensors')
    .then(resp => resp.json())
    .then(json => {
      sensorStore.sensors = json ;
    })
    .catch(err => {
      console.error(err);
      // createToast('Error fetching /sensors', TOAST_ERROR);
      // return;
    });

  await fetch(APP_API_URL + '/system')
    .then(resp => resp.json())
    .then(json => {
      appStore.system = json;
    })
    .catch(err => {
      console.error(err);
      // createToast('Error fetching /system', TOAST_ERROR);
      // return;
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
