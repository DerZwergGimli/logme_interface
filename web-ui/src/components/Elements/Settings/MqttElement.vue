<script setup lang="ts">
import { useAppStore } from '../../../stores/AppStore';
import { Button } from 'flowbite-vue';
</script>
<template>
  <table id="tableTABLE">
    <thead id="tableHEAD">
      <tr>
        <th>URL</th>
        <th>Username</th>
        <th>Password</th>
        <th>Enabled</th>
        <th></th>
      </tr>
    </thead>
    <tbody>
      <tr>
        <td>{{ useAppStore().mqtt.url }}</td>
        <td>{{ useAppStore().mqtt.username }}</td>
        <td>{{ useAppStore().mqtt.password }}</td>
        <td>{{ useAppStore().mqtt.enabled }}</td>
        <td class="float-right"><Button>EDIT</Button></td>
      </tr>
    </tbody>
  </table>
</template>
