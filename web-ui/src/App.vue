<script setup lang="ts">
import HeaderComponent from './components/header_footer/HeaderComponent.vue';
import FooterComponent from './components/header_footer/FooterComponent.vue';
import { useSensorStore } from "./stores/SensorStore";

const sensorStore = useSensorStore()
sensorStore.fetch()
</script>

<template>
  <div class="flex flex-col justify-between min-h-screen">
    <header-component class="border-b mb-3 sticky top-0 z-50" />
    <router-view class="mb-auto"></router-view>
    <footer-component class="w-full bottom-0" />
  </div>
</template>
