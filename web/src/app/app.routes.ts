import { Routes } from '@angular/router';
import { RelayPage } from './features/relay/relay.page';
import { SettingsPage } from './features/settings/settings.page';

export const routes: Routes = [
  {
    path: '',
    component: RelayPage,
  },
  {
    path: 'settings',
    component: SettingsPage,
  },
  {
    path: '**',
    redirectTo: '',
  },
];
