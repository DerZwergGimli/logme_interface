<script setup lang="ts">

import { PropType } from "vue";

const props = defineProps({
    value: {
    type: String,
    default: "--.--",
  },
  unit: {
    type: String,
    default: 'xXx',
  },
  description: {
    type: String,
    default: 'SomeDescription',
  },
});

</script>

<template>

  <div
    class="flex flex-row space-x-2 rounded-lg border-2 dark:border-black dark:shadow-inner p-1 dark:text-white bg-gradient-to-r from-indigo-400 to-pink-400 dark:from-indigo-800 dark:to-pink-800"
  >
    <div class="flex m-2 items-center justify-center">

    </div>
    <div class="flex flex-col w-full justify-center">
      <div class="flex flex-row space-x-1">
        <div>{{ props.value }}</div>

        <div>{{ props.unit.replace(')', '') }}</div>
      </div>
      <div>{{ props.description }}</div>
    </div>
  </div>

</template>
