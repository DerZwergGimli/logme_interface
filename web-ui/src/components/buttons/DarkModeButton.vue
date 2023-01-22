<template>
  <button
    @click="appStore.theme_toggle()"
    type="button"
    class="text-black dark:text-white hover:bg-blue-200 focus:ring-4 focus:ring-blue-300 font-medium rounded-lg p-2 dark:hover:bg-blue-700 focus:outline-none dark:focus:ring-blue-800"
  >
    <dark-icon v-if="!appStore.themeIsDark"></dark-icon>
    <light-icon v-else />
  </button>
</template>

<script setup lang="ts">
import DarkIcon from '../icons/DarkIcon.vue';
import LightIcon from '../icons/LightIcon.vue';
import { useAppStore } from '../../stores/AppStore';

const appStore = useAppStore()
appStore.init()
</script>
