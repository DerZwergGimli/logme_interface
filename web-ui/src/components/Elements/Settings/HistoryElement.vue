<script setup lang="ts">
import { useAppStore } from '../../../stores/AppStore';
import { Button } from 'flowbite-vue';
import { action_post } from '../../../scripts/action_commands';
</script>
<template>
  <table id="tableTABLE">
    <thead id="tableHEAD">
      <tr>
        <th>Action</th>
        <th></th>
      </tr>
    </thead>
    <tbody>
      <tr>
        <td>Force SAVE history to flash</td>
        <td class="float-right">
          <Button
            class="w-24"
            color="yellow"
            @click="action_post('/sensorsave')"
            >Save</Button
          >
        </td>
      </tr>
      <tr>
        <td>Force CLEAR history in flash</td>
        <td class="float-right">
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
</template>
