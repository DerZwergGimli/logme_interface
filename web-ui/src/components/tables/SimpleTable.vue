<template>
  <div class="overflow-x-auto relative">
    <table>
      <thead>
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
        <tr v-for="(element, idx) in props.body" :key="idx">
          <td v-for="e in element" :key="e">{{ e }}</td>
          <td v-if="props.button_text" class="float-right">
            <Button @click="$emit('button_clicked', element)">
              {{ props.button_text }}
            </Button>
          </td>
        </tr>
      </tbody>
    </table>
  </div>
</template>

<script setup lang="ts">
import { defineProps } from 'vue';
import { Button } from 'flowbite-vue';
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
