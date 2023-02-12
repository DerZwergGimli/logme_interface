<script setup lang="ts">
import { useAppStore } from '../../../stores/AppStore';
import { Progress, Button } from 'flowbite-vue';
import { createToast } from 'mosha-vue-toastify';
import { TOAST_ERROR, TOAST_INFO } from '../../../scripts/toast_config';

function send_post(url: String) {
  fetch(APP_API_URL + url, {
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
      createToast('Error sending ' + url, TOAST_ERROR);
    });
}

function convert_time(time_ms: number) {
  if (time_ms / 1000 / 60 / 60 / 24 > 1) {
    return (time_ms / 1000 / 60 / 60 / 24).toFixed(2) + ' days';
  }
  if (time_ms / 1000 / 60 / 60 > 1) {
    return (time_ms / 1000 / 60 / 60).toFixed(2) + ' h';
  }
  if (time_ms / 1000 / 60 > 1) {
    return (time_ms / 1000 / 60).toFixed(2) + ' min';
  }
  if (time_ms / 1000 > 0) {
    return (time_ms / 1000).toFixed(2) + ' s';
  }
}
</script>
<template>
  <table id="tableTABLE">
    <thead id="tableHEAD">
      <tr>
        <th>Element</th>
        <th>Value</th>
      </tr>
    </thead>
    <tbody>
      <tr>
        <td>Target</td>
        <td>{{ useAppStore().system?.info?.target }}</td>
      </tr>
      <tr>
        <td>Chip Version</td>
        <td>
          {{
            useAppStore().system?.info?.major_rev +
            '.' +
            useAppStore().system?.info?.minor_rev
          }}
        </td>
      </tr>
      <tr>
        <td>CPU</td>
        <td>{{ useAppStore().system?.info?.cores + ' Cores' }}</td>
      </tr>
      <tr>
        <td>Flash size</td>
        <td>{{ useAppStore().system?.info?.flash_size_MB + ' MB' }}</td>
      </tr>

      <tr>
        <td>Uptime</td>
        <td>{{ convert_time(useAppStore().system?.info?.uptime_ms ?? 0) }}</td>
      </tr>
      <tr>
        <td>Time [UTC]</td>
        <td>{{ useAppStore().system?.info?.time }}</td>
      </tr>
      <tr>
        <td>Heap</td>
        <td>
          <div>
            <Progress
              :labelProgress="true"
              labelPosition="outside"
              label="Free Heap"
              :progress="
                parseInt(
                  (
                    ((useAppStore().system?.info?.free_heap ?? 0) /
                      (useAppStore().system?.info?.total_heap ?? 0)) *
                    100
                  ).toFixed(0)
                )
              "
            ></Progress>
          </div>
        </td>
      </tr>
      <tr>
        <td>Wifi-Reset</td>
        <td>
          <Button class="float-right" @click="send_post('/wifireset')"
            >Reset</Button
          >
        </td>
      </tr>
      <tr>
        <td>Restart</td>
        <td>
          <Button class="float-right" @click="send_post('/restart')"
            >Restart</Button
          >
        </td>
      </tr>
    </tbody>
  </table>
</template>
