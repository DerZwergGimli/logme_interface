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
        <td>{{ useAppStore().system?.mqtt?.host_url }}</td>
        <td>{{ useAppStore().system?.mqtt?.username }}</td>
        <td>{{ useAppStore().system?.mqtt?.password }}</td>
        <td>{{ useAppStore().system?.mqtt?.enable }}</td>
        <td><Button>EDIT</Button></td>
      </tr>
    </tbody>
  </table>
</template>
