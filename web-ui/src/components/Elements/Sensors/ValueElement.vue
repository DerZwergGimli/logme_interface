<template>
  <div
    class="flex flex-row space-x-2 rounded-lg border-2 dark:border-black dark:shadow-inner p-1 dark:text-white dark:bg-gradient-to-r dark:from-indigo-500 dark:via-purple-500 dark:to-pink-500"
  >
    <div class="flex m-2 items-center justify-center">
      <div v-if="props.icon === 'info'">
        <info-icon />
      </div>
      <div v-if="props.icon === 'id'">
        <id-icon />
      </div>
      <div v-if="props.icon === 'numbers'">
        <numbers-icon />
      </div>
      <div v-if="props.icon === 'power-plug'">
        <power-plug-icon />
      </div>
    </div>
    <div class="flex flex-col">
      <div class="flex flex-row space-x-1">
        <div>{{ props.value }}</div>
        <div>{{ props.unit }}</div>
      </div>
      <div>{{ props.description }}</div>
    </div>
  </div>
</template>

<script setup lang="ts">
import InfoIcon from '../../icons/InfoIcon.vue';
import IdIcon from '../../icons/IdIcon.vue';
import NumbersIcon from '../../icons/NumbersIcon.vue';
import PowerPlugIcon from '../../icons/PowerPlugIcon.vue';

const props = defineProps({
  icon: {
    type: String,
    default: 'info',
  },
  value: {
    type: String,
    default: 99999,
  },
  unit: {
    type: String,
    default: 'kWh',
  },
  description: {
    type: String,
    default: 'SomeDescription',
  },
});
</script>

<style scoped></style>
