<script setup lang="ts">
import { useAppStore } from '../../../stores/AppStore';
import { Button } from 'flowbite-vue';
import { createToast } from 'mosha-vue-toastify';
import { TOAST_ERROR, TOAST_INFO } from '../../../scripts/toast_config';

function action_send_mqtt_ping() {
  fetch(APP_API_URL + '/mqtt/ping', {
    method: 'POST',
    headers: {
      Accept: 'application/json',
      'Content-Type': 'application/json',
    },
  })
    .then(response => response.json())
    .then(response => {
      console.log(response);
      createToast(response.message, TOAST_INFO);
    })
    .catch(err => {
      console.error(err);
      createToast('Error sending ping!', TOAST_ERROR);
    });
}
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
        <th></th>
      </tr>
    </thead>
    <tbody>
      <tr>
        <td>
          {{
            useAppStore().mqtt.protocol +
            '://' +
            useAppStore().mqtt.ip +
            ':' +
            useAppStore().mqtt.port
          }}
        </td>
        <td>{{ useAppStore().mqtt.username }}</td>
        <td>{{ useAppStore().mqtt.password }}</td>
        <td>{{ useAppStore().mqtt.enabled }}</td>
        <td>
          <Button
            class="float-right"
            color="purple"
            @click="action_send_mqtt_ping()"
            >Ping</Button
          >
        </td>
        <td><Button class="float-right">EDIT</Button></td>
      </tr>
    </tbody>
  </table>
</template>
