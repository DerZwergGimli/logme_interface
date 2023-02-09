<template>
  <footer
    class="mt-5 p-4 bg-white shadow md:flex md:items-center md:justify-between md:p-6 dark:bg-gray-800"
  >
    <div class="flex flex-row w-full items-center">
      <p class="flex flex-row text-sm text-gray-500 dark:text-gray-400">
        © 2023 LogME. All Rights Reserved.
      </p>
      <div class="w-full"></div>
      <Progress
        v-if="useAppStore().showHeap"
        class="basis-1/2"
        :progress="useAppStore().heapPercentage"
        :labelProgress="false"
        labelPosition="inside"
        size="lg"
      ></Progress>
      <div class="w-full"></div>
      <a
        class="flex hover:underline justify-end"
        href="flex https://github.com/DerZwergGimli/logme_interface"
        ><github-icon class="dark:text-gray-100"
      /></a>
    </div>
  </footer>
</template>
<script setup lang="ts">
import GithubIcon from '../icons/GithubIcon.vue';
import { Progress } from 'flowbite-vue';
import { useAppStore } from '../../stores/AppStore.js';
</script>
