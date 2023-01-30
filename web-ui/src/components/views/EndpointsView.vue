<template>
  <div>
    <div class="flex flex-col space-y-5 pb-5">
      <SimpleTable
        id="glowElement"
        :header="['REST GET-Requests', 'Description']"
        :body="[
          ['/system', 'Requests a system-info object'],
          ['/wifi', 'Requests a wifi-info object'],
          ['/sensors', 'Requests the sensors object'],
          ['/sensorstore.json', 'Requests the sensors-store form flash'],
        ]"
        :button_text="'GET'"
        @button_clicked="handle_get_button"
      />
      <SimpleTable
        id="glowElement"
        :header="['REST POST-Requests', 'Description']"
        :body="[
          ['/restart', 'Force a device restart'],
          ['/wifireset', 'Force a WiFi-config reset'],
          ['/wificonfig', 'Writes a given wifi-config'],
          ['/sensorsave', 'Force save sensor-config'],
          ['/sensorhistoryclear', 'Force delete all sensor-history'],
        ]"
        :button_text="'POST'"
      />
      <SimpleTable
        id="glowElement"
        :header="['Websocket']"
        :body="[['/websocket']]"
        :button_text="'WS'"
      />
    </div>
  </div>
</template>
<script setup lang="ts">
import SimpleTable from '../tables/SimpleTable.vue';

function handle_get_button(event: [string]) {
  window.open(event[0], '_blank');
}
</script>

<style scoped>

</style>
