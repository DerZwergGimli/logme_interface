<template>
  <div
    class="flex flex-row space-x-3 bg-yellow-400 dark:bg-yellow-700 rounded-lg p-3 items-center justify-center"
  >
    <warn-icon class="w-6 dark:text-white" />
    <p>{{ props.text }}</p>
    <warn-icon class="w-6 dark:text-white" />
  </div>
</template>
<script setup lang="ts">
import WarnIcon from '../../icons/WarnIcon.vue';

const props = defineProps({
  text: {
    type: String,
    default: 'NO DATA',
  },
});
</script>
