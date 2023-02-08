<template>
  <footer
    class="mt-5 p-4 bg-white shadow md:flex md:items-center md:justify-between md:p-6 dark:bg-gray-800"
  >
    <div class="flex flex-row w-full items-center justify-around">
      <p class="text-sm text-gray-500 sm:text-center dark:text-gray-400">
        © 2023 <a href="#" class="hover:underline">LogME</a>. All Rights
        Reserved.
      </p>

      <Progress
        class="basis-1/2"
        :progress="useAppStore().heapPercentage"
        :labelProgress="false"
        labelPosition="inside"
        size="lg"
      ></Progress>
      <a
        href="https://github.com/DerZwergGimli/logme_interface"
        class="items-center hover:underline"
        ><github-icon
      /></a>
    </div>
  </footer>
</template>
<script setup lang="ts">
import GithubIcon from '../icons/GithubIcon.vue';
import { Progress } from 'flowbite-vue';
import { useAppStore } from '../../stores/AppStore.js';
</script>
