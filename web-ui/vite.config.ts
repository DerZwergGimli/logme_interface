import { defineConfig } from 'vite';
import vue from '@vitejs/plugin-vue';

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [vue()],
  define: {
    APP_VERSION: JSON.stringify(process.env.npm_package_version),
    APP_API_URL: JSON.stringify(
      process.env.NODE_ENV === 'production' ? '' : 'http://localhost:3000'
    ),
  },
});
