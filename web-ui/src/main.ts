import { createApp } from 'vue';
import './style.css';
import 'flowbite';
import './main.css';
import App from './App.vue';
import { createPinia } from 'pinia';
import { createRouter, createWebHashHistory } from 'vue-router';
import VueApexCharts from 'vue3-apexcharts';
import moshaToast from 'mosha-vue-toastify';
import 'mosha-vue-toastify/dist/style.css';
import 'flowbite';
import 'uno.css';

import HomeView from './components/views/HomeView.vue';
import SensorView from './components/views/SensorView.vue';
import EndpointsView from './components/views/EndpointsView.vue';
import SettingsView from './components/views/SettingsView.vue';
import { useAppStore } from './stores/AppStore';
import { useSensorStore } from './stores/SensorStore';

const routes = [
  { path: '/', component: HomeView },
  { path: '/sensors', component: SensorView },
  { path: '/endpoints', component: EndpointsView },
  { path: '/settings', component: SettingsView },
];

const router = createRouter({
  history: createWebHashHistory(),
  routes,
});

const pina = createPinia();
const app = createApp(App);

app.use(router).use(pina).use(VueApexCharts).use(moshaToast).mount('#app');
