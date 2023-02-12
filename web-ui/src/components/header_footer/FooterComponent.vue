<template>
  <footer class="mt-5 p-4 bg-white shadow md:p-6 dark:bg-gray-800">
    <div class="flex flex-row w-full items-center justify-around">
      <div class="flex text-sm text-gray-500 dark:text-gray-400">
        © 2023 LogME. All Rights Reserved.
      </div>

      <Progress
        v-if="useAppStore().showHeap"
        class="flex justify-center basis-1/2"
        :progress="useAppStore().heapPercentage"
        :labelProgress="false"
        labelPosition="inside"
        size="lg"
      ></Progress>
      <a
        class="flex hover:underline float-right"
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
