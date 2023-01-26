<script setup lang="ts">
import { Button } from 'flowbite-vue';
import { createToast } from 'mosha-vue-toastify';
import { TOAST_ERROR, TOAST_INFO } from '../../scripts/toast_config';
import { action_post } from '../../scripts/action_commands';
</script>
<template>
  <div class="overflow-x-auto relative rounded-md">
    <table class="w-full text-sm text-left text-gray-500 dark:text-gray-400">
      <thead
        class="text-xs text-gray-700 uppercase bg-gray-50 dark:bg-gray-700 dark:text-gray-400"
      >
        <tr>
          <th scope="col" class="py-3 px-6">Actions</th>
          <th scope="col" class="py-3 px-6"></th>
        </tr>
      </thead>
      <tbody>
        <tr class="bg-white border-b dark:bg-gray-800 dark:border-gray-700">
          <td class="py-4 px-6">Save-History to Flash</td>
          <td class="py-4 px-6 float-right">
            <Button
              class="w-24"
              color="yellow"
              @click="action_post('/sensorsave')"
              >Save</Button
            >
          </td>
        </tr>
        <tr class="bg-white border-b dark:bg-gray-800 dark:border-gray-700">
          <td class="py-4 px-6">Clear-History in RAM</td>
          <td class="py-4 px-6 float-right">
            <Button
              class="w-24"
              color="red"
              @click="action_post('/sensorhistoryclear')"
              >Clear</Button
            >
          </td>
        </tr>
      </tbody>
    </table>
  </div>
</template>
