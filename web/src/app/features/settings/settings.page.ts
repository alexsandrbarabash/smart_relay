import { Component, OnInit } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { Router } from '@angular/router';
import { LucideArrowLeft, LucideBot, LucideClock, LucideRouter, LucideWifi } from '@lucide/angular';
import { AppShell } from '../../shared/components/app-shell/app-shell';
import { ActionSwitch } from '../../shared/components/action-switch/action-switch';
import { Field } from '../../shared/components/field/field';
import { IconButton } from '../../shared/components/icon-button/icon-button';
import { PrimaryButton } from '../../shared/components/primary-button/primary-button';
import { SettingsVm } from './settings.vm';

@Component({
  selector: 'app-settings-page',
  standalone: true,
  imports: [
    FormsModule,
    LucideArrowLeft,
    LucideBot,
    LucideClock,
    LucideRouter,
    LucideWifi,
    AppShell,
    ActionSwitch,
    Field,
    IconButton,
    PrimaryButton,
  ],
  providers: [SettingsVm],
  templateUrl: './settings.page.html',
  styleUrl: './settings.page.scss',
})
export class SettingsPage implements OnInit {
  constructor(
    protected readonly vm: SettingsVm,
    private readonly router: Router,
  ) {}

  ngOnInit(): void {
    void this.vm.load();
  }

  backToRelay(): void {
    void this.router.navigateByUrl('/');
  }
}
