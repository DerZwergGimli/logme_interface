<script setup lang="ts">
import { Button } from 'flowbite-vue';
import { Progress } from 'flowbite-vue';
import { defineProps } from 'vue';
import { action_post } from '../../scripts/action_commands';

const props = defineProps({
  header: {
    type: Array,
    default: ['System', 'Value'],
  },
  body: {
    default: [
      ['Target', '---'],
      ['Chip Version', '---'],
      ['CPU', '---'],
      ['Flash size', '---'],
      ['HEAP', '---'],
      ['Uptime', '---'],
    ],
  },
});

function calc_percentage(a: number, b: number): number {
  let percentage = (a / b) * 100;
  return parseFloat(percentage.toFixed(1));
}
</script>

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
        </tr>
      </thead>
      <tbody>
        <tr
          v-for="(element, idx) in props.body"
          :key="idx"
          class="bg-white border-b dark:bg-gray-800 dark:border-gray-700"
        >
          <td v-if="element[0] === 'HEAP'" class="py-4 px-6">HEAP</td>
          <td v-if="element[0] === 'HEAP'" class="py-4 px-6">
            <Progress
              :progress="
                calc_percentage(parseInt(element[2]), parseInt(element[1]))
              "
              :labelProgress="true"
              labelPosition="inside"
              size="lg"
            ></Progress>
            <div class="flex flex-row mx-1">
              <div class="flex w-full">USED/TOTAL</div>
              <div class="text-right">
                {{ parseInt(element[2]) }}/ {{ parseInt(element[1]) }}
              </div>
            </div>
          </td>
          <td v-else v-for="e in element" :key="e" class="py-4 px-6">
            {{ e }}
          </td>
        </tr>
        <tr class="bg-white border-b dark:bg-gray-800 dark:border-gray-700">
          <td class="py-4 px-6">Reset-WiFi</td>
          <td class="py-4 px-6 float-right">
            <Button class="w-24" color="red" @click="action_post('/restart')"
              >Reset</Button
            >
          </td>
        </tr>
        <tr class="bg-white border-b dark:bg-gray-800 dark:border-gray-700">
          <td class="py-4 px-6">Restart</td>
          <td class="py-4 px-6 float-right">
            <Button class="w-24" color="yellow" @click="action_post('/restart')"
              >Restart</Button
            >
          </td>
        </tr>
      </tbody>
    </table>
  </div>
</template>
