<script setup lang="ts">
import { onMounted, ref } from 'vue';
import SimpleTable from '../../tables/SimpleTable.vue';
import { I_AccessPoint } from '../../../stores/interfaces/T_AccessPoint';
import { Button } from 'flowbite-vue';
import ReloadIcon from '../../icons/ReloadIcon.vue';
import WifiCredentialsModal from '../../modals/WifiCredentialsModal.vue';
import { createToast } from 'mosha-vue-toastify';
import { TOAST_INFO } from '../../../scripts/toast_config';
import { useAppStore } from '../../../stores/AppStore';

const modalShown = ref(false);
const selected_network_ssid = ref('network_ssid');

const wifi_config_table_header = ref(['WiFi', 'Value']);
const wifi_config_table_content = ref([
  ['SSID', '---'],
  ['IP', '---'],
  ['Netmask', '---'],
  ['Gateway', '---'],
]);

const networks_table_header = ref(['SSID', 'RSSI', 'AUTH', 'CHANNEL']);
const networks_table_content = ref([]);

onMounted(async () => {
  fetch_endpoint_wifi();
  fetch_endpoint_ap();
});

function fetch_endpoint_wifi() {
  // FETCH WIFI ENDPOINT
  fetch(APP_API_URL + '/wifi')
    .then(resp => resp.json())
    .then(json => {
      console.info(json);
      wifi_config_table_content.value[0][1] = json.ssid;
      wifi_config_table_content.value[1][1] = json.ip;
      wifi_config_table_content.value[2][1] = json.netmask;
      wifi_config_table_content.value[3][1] = json.gateway;
    })
    .catch(err => console.error(err));
}

function fetch_endpoint_ap() {
  // FETCH AP ENDPOINT
  fetch(APP_API_URL + '/ap')
    .then(resp => resp.json())
    .then(json => {
      useAppStore().wifi = {
        aps: json,
        config: useAppStore().wifi?.config,
      };
    })
    .catch(err => console.error(err));
}

function action_wifi_modal(network_ssid: String) {
  selected_network_ssid.value = network_ssid.toString();
  modalShown.value = true;
}
</script>

<template>
  <div class="flex flex-col space-y-5">
    <table id="tableTABLE">
      <thead id="tableHEAD">
        <tr>
          <th>WiFi-Config</th>
          <th>Value</th>
        </tr>
      </thead>
      <tbody>
        <tr>
          <td>SSID</td>
          <td>{{ useAppStore().wifi?.config?.ssid }}</td>
        </tr>
        <tr>
          <td>IP</td>
          <td>{{ useAppStore().wifi?.config?.ip }}</td>
        </tr>
        <tr>
          <td>Netmask</td>
          <td>{{ useAppStore().wifi?.config?.netmask }}</td>
        </tr>
        <tr>
          <td>Gateway</td>
          <td>{{ useAppStore().wifi?.config?.gateway }}</td>
        </tr>
      </tbody>
    </table>

    <table id="tableTABLE">
      <thead id="tableHEAD">
        <tr>
          <th>WiFi-AccessPoints</th>
          <th>RSSI</th>
          <th>AUTH</th>
          <th>Channel</th>
          <th class="">
            <Button
              class="float-right"
              @click="
                () => {
                  createToast('Updating Networks', TOAST_INFO);
                  fetch_endpoint_ap();
                }
              "
              color="default"
              ><reload-icon
            /></Button>
          </th>
        </tr>
      </thead>
      <tbody>
        <tr v-for="(ap, index) in useAppStore().wifi?.aps" :key="index">
          <td>{{ ap.ssid }}</td>
          <td>{{ ap.rssi }}</td>
          <td>{{ ap.auth }}</td>
          <td>{{ ap.chan }}</td>
          <td class="float-right">
            <Button @click="() => action_wifi_modal(ap.ssid)">Connect</Button>
          </td>
        </tr>
      </tbody>
    </table>
  </div>

  <div v-if="modalShown">
    <wifi-credentials-modal
      :is-shown="modalShown"
      :ssid="selected_network_ssid"
      @closeModal="modalShown = false"
    />
  </div>
</template>
