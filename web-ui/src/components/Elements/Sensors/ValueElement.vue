<template>
  <div
    class="flex flex-row space-x-2 rounded-lg border-2 dark:border-black dark:shadow-inner p-1 dark:text-white bg-gradient-to-r from-indigo-400 to-pink-400 dark:from-indigo-800 dark:to-pink-800"
  >
    <div class="flex m-2 items-center justify-center">
      <div v-if="props.icon === 'info'">
        <info-icon class="" />
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
    <div class="flex flex-col w-full justify-center">
      <div class="flex flex-row space-x-1">
        <div>{{ props.value }}</div>

        <div>{{ props.unit }}</div>
      </div>
      <div>{{ props.description }}</div>
    </div>

    <Button pill color="light" v-if="props.editable" @click="action_edit()"
      ><edit-icon
    /></Button>
  </div>
</template>

<script setup lang="ts">
import InfoIcon from '../../icons/InfoIcon.vue';
import IdIcon from '../../icons/IdIcon.vue';
import NumbersIcon from '../../icons/NumbersIcon.vue';
import PowerPlugIcon from '../../icons/PowerPlugIcon.vue';
import { Button } from 'flowbite-vue';
import EditIcon from '../../icons/EditIcon.vue';

defineEmits(['buttonClick']);

const props = defineProps({
  editable: {
    type: Boolean,
    default: true,
  },
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

function action_edit() {}
</script>

<style scoped></style>
