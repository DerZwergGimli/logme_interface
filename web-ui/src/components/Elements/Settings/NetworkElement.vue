<template>
  <div>
    <div class="space-y-10">
      <div>
        <h3 class="text-center dark:text-white underline pb-2">WiFi Config</h3>
        <simple-table
          :header="wifi_config_table_header"
          :body="wifi_config_table_content"
        />
      </div>

      <div>
        <h3 class="text-center dark:text-white underline pb-2">
          Available Networks
        </h3>
        <simple-table
          :header="networks_table_header"
          :body="networks_table_content"
          :button_text="'Connect'"
        />
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { onMounted, ref } from 'vue';
import SimpleTable from '../../tables/SimpleTable.vue';

const wifi_config_table_header = ref(['WiFi', 'Value']);
const wifi_config_table_content = ref([
  ['SSID', '---'],
  ['RSSI', '---'],
  ['IP', '---'],
  ['Netmask', '---'],
  ['Gateway', '---'],
]);

const networks_table_header = ref(['SSID', 'RSSI', '']);
const networks_table_content = ref([]);

onMounted(async () => {
  fetch(APP_API_URL + '/wifi')
    .then(resp => resp.json())
    .then(json => {
      console.info(json);
      wifi_config_table_content.value[0][1] = json.status.ssid;
      wifi_config_table_content.value[1][1] = json.status.rssi;
      wifi_config_table_content.value[2][1] = json.status.ip;
      wifi_config_table_content.value[3][1] = json.status.netmask;
      wifi_config_table_content.value[4][1] = json.status.gateway;

      json.networks.forEach((nw: any) => {
        if (nw.ssid != '') {
          networks_table_content.value.push([nw.ssid, nw.rssi, ''] as never);
        }
      });
    })
    .catch(err => console.error(err));
});
</script>
