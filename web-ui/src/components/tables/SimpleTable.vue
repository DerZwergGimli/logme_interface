<template>
  <div class="overflow-x-auto relative rounded-md">
    <table class="w-full text-sm text-left text-gray-500 dark:text-gray-400">
      <thead
        class="text-xs text-gray-700 uppercase bg-gray-50 dark:bg-gray-700 dark:text-gray-400"
      >
        <tr>
          <th
            v-for="(element, idx) in props.header"
            :key="idx"
            scope="col"
            class="py-3 px-6"
          >
            {{ element }}
          </th>
          <th v-if="props.button_text"></th>
        </tr>
      </thead>
      <tbody>
        <tr
          v-for="(element, idx) in props.body"
          :key="idx"
          class="bg-white border-b dark:bg-gray-800 dark:border-gray-700"
        >
          <td v-for="e in element" :key="e" class="py-4 px-6">{{ e }}</td>
          <td v-if="props.button_text" class="flex py-4 px-6 float-right">
            <button
              @click="$emit('button_clicked', element)"
              type="button"
              class="text-white bg-blue-700 hover:bg-blue-800 focus:ring-4 focus:ring-blue-300 font-medium rounded-lg text-sm px-5 py-2.5 mr-2 mb-2 dark:bg-blue-600 dark:hover:bg-blue-700 focus:outline-none dark:focus:ring-blue-800"
            >
              {{ props.button_text }}
            </button>
          </td>
        </tr>
      </tbody>
    </table>
  </div>
</template>

<script setup lang="ts">
import { defineProps } from 'vue';
const props = defineProps({
  header: {
    type: Array,
    default: ['h_0', 'h_1', 'h_2'],
  },
  body: {
    type: Array,
    default: [
      ['c_00', 'c_01', 'c_02'],
      ['c_10', 'c_11', 'c_12'],
      ['c_20', 'c_21', 'c_22'],
    ],
  },
  button_text: {
    type: String,
    default: undefined,
  },
});
defineEmits(['button_clicked']);
</script>
